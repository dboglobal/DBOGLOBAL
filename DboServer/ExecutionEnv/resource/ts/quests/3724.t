CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3724;
	title = 372402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372409;
				ctype = 1;
				idx = 3172210;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 16421200;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 16421101;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 372408;
				m1fx = "-3353.030029";
				m2fz = "-4363.240234";
				m1fy = "-63.430000";
				sort = 372405;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 372401;
				goal = 372404;
				m0fz = "-4418.930176";
				m0widx = 15;
				m1ttip = 372416;
				m2fy = "-63.430000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "-64.160004";
				m0pat = 1;
				m1fz = "-4202.770020";
				m2fx = "-3401.939941";
				m2ttip = 372416;
				m0fx = "-3428.580078";
				m0ttip = 372415;
				title = 372402;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 372408;
			gtype = 3;
			oklnk = 2;
			area = 372401;
			goal = 372404;
			sort = 372405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372402;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
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
				itype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 372407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3172210;";
			}
			CDboTSCheckClrQst
			{
				and = "3723;";
				flink = 0;
				flinknextqid = "3725;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372414;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 372401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7076104;";
			}
		}
	}
}

