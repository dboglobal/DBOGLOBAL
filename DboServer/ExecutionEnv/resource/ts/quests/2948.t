CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2948;
	title = 294802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 2948;
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
				id = 701;
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
				sid = 2948;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 294814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 294801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4481204;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 6;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 294809;
				ctype = 1;
				idx = 4511313;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 294808;
				area = 294801;
				goal = 294804;
				m0fz = "-3860.290039";
				m0widx = 1;
				scitem = -1;
				sort = 294805;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "3151.000000";
				m0ttip = 294815;
				rwd = 100;
				taid = 1;
				title = 294802;
				gtype = 3;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 294808;
			gtype = 3;
			oklnk = 2;
			area = 294801;
			goal = 294804;
			sort = 294805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 294802;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 294807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSClickNPC
			{
				npcidx = "4511313;";
			}
			CDboTSCheckClrQst
			{
				and = "2947;";
				flink = 1;
				flinknextqid = "2949;";
				not = 0;
			}
		}
	}
}

