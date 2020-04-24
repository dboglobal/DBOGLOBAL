CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2927;
	title = 292702;

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
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 2927;
				start = 0;
				taid = 1;
				uof = 0;
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
				id = 697;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 253;
			nextlnk = 4;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 2927;
				start = 1;
				taid = 1;
				uof = 1;
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
				conv = 292709;
				ctype = 1;
				idx = 5042105;
				taid = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 292708;
				area = 292701;
				goal = 292704;
				m0fz = "-3850.750000";
				m0widx = 1;
				scitem = -1;
				sort = 292705;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = -1;
				m0fx = "2687.639893";
				m0ttip = 292715;
				rwd = 100;
				taid = 3;
				title = 292702;
				gtype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "3;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 292707;
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
				npcidx = "5042105;";
			}
			CDboTSCheckClrQst
			{
				and = "2926;";
				flink = 0;
				flinknextqid = "2928;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 292714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 292701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5042105;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 292708;
			gtype = 2;
			oklnk = 2;
			area = 292701;
			goal = 292704;
			sort = 292705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 292702;
		}
	}
}

