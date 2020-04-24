CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2910;
	title = 291002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 6;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 291009;
				ctype = 1;
				idx = 6111205;
				taid = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 291008;
				area = 291001;
				goal = 291004;
				m0fz = "-2309.820068";
				m0widx = 1;
				scitem = -1;
				sort = 291005;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "2248.840088";
				m0ttip = 291015;
				rwd = 100;
				taid = 3;
				title = 291002;
				gtype = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 291007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "6111205;";
			}
			CDboTSCheckClrQst
			{
				and = "2909;";
				flink = 0;
				flinknextqid = "2911;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;3;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 253;
			nextlnk = 4;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 2910;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 694;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 2910;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 291008;
			gtype = 2;
			oklnk = 2;
			area = 291001;
			goal = 291004;
			sort = 291005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 291002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 291014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 291001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111205;";
			}
		}
	}
}

