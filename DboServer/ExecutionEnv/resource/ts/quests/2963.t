CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2963;
	title = 296302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1536;
				taid = 1;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 296309;
				ctype = 1;
				idx = 4751210;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1536;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 296308;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = -1;
				m1pat = 2;
				m1widx = 1;
				m3fx = "4517.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "4533.000000";
				m4ttip = 296315;
				area = 296301;
				goal = 296304;
				m0fz = "-4568.000000";
				m0widx = 1;
				m1ttip = 296315;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-4468.000000";
				m3ttip = 296315;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4512.000000";
				m2fx = "4534.000000";
				m2ttip = 296315;
				m4widx = 1;
				m0fx = "4304.000000";
				m0ttip = 296315;
				m3pat = 2;
				m4pat = 2;
				title = 296302;
				m1fx = "4290.000000";
				m2fz = "-4342.000000";
				sort = 296305;
				m4fz = "-4611.000000";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 296314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 296301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751210;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 296307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "4751210;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 296308;
			gtype = 2;
			oklnk = 2;
			area = 296301;
			goal = 296304;
			sort = 296305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 296302;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1536;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1536;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 1;
		}
	}
}

