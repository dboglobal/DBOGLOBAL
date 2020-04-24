CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3030;
	title = 303002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 303007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 59;
			}
			CDboTSClickNPC
			{
				npcidx = "4953102;";
			}
			CDboTSCheckClrQst
			{
				and = "3029;";
				flink = 0;
				flinknextqid = "3031;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
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
			desc = 303014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 303001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4953102;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 303008;
			gtype = 2;
			oklnk = 2;
			area = 303001;
			goal = 303004;
			sort = 303005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 303002;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 303009;
				ctype = 1;
				idx = 4953102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 15931101;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 15931301;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 303008;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "3440.479980";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "2945.709961";
				m4ttip = 303016;
				area = 303001;
				goal = 303004;
				m0fz = "-5032.100098";
				m0widx = 1;
				m1ttip = 303015;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-5021.189941";
				m3ttip = 303016;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4851.390137";
				m2fx = "3313.020020";
				m2ttip = 303015;
				m4widx = 1;
				m0fx = "2990.699951";
				m0ttip = 303015;
				m3pat = 2;
				m4pat = 2;
				title = 303002;
				m1fx = "2969.649902";
				m2fz = "-4990.830078";
				sort = 303005;
				m4fz = "-5031.959961";
			}
		}
	}
}

