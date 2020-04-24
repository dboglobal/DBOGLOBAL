CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3723;
	title = 372302;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 372307;
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
				npcidx = "7076104;";
			}
			CDboTSCheckClrQst
			{
				and = "3722;";
				flink = 0;
				flinknextqid = "3724;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372309;
				ctype = 1;
				idx = 7076104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 35;
				eitype = 0;
				idx0 = 17512100;
				cnt0 = 35;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 11721105;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 372308;
				m3widx = 15;
				m1fy = "-33.419998";
				sort = 372305;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-3479.060059";
				rwd = 100;
				taid = 1;
				gtype = 1;
				m3fy = "-49.849998";
				area = 372301;
				goal = 372304;
				m0fz = "-4440.000000";
				m0widx = 15;
				m1ttip = 372315;
				m2fy = "-41.549999";
				m2pat = 2;
				m3fz = "-4700.020020";
				m3ttip = 372316;
				scitem = -1;
				stype = 2;
				m0fy = "-38.110001";
				m0pat = 2;
				m1fz = "-4256.899902";
				m2fx = "-3195.600098";
				m2ttip = 372316;
				m0fx = "-3151.280029";
				m0ttip = 372315;
				m3pat = 2;
				title = 372302;
				m1fx = "-3640.899902";
				m2fz = "-4561.609863";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 372308;
			gtype = 1;
			oklnk = 2;
			area = 372301;
			goal = 372304;
			sort = 372305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372302;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372314;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 372301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172210;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
	}
}

