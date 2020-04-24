CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 23;
	rq = 0;
	tid = 6903;
	title = 690302;

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
			cont = 690308;
			gtype = 1;
			oklnk = 2;
			area = 690301;
			goal = 690304;
			sort = 690305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 690302;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 690314;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 690301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3185102;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 690307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSCheckClrQst
			{
				and = "6902;";
				flink = 0;
				not = 0;
			}
			CDboTSCheckPCRace
			{
				raceflg = 7;
			}
			CDboTSClickNPC
			{
				npcidx = "3185102;";
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
				conv = 690309;
				ctype = 1;
				idx = 3185102;
				taid = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 51732100;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 51762600;
				taid = 2;
				esctype = 0;
				idx2 = 58131100;
			}
			CDboTSActRegQInfo
			{
				cont = 690308;
				m1fx = "4736.620117";
				m2fz = "-798.109985";
				m1fy = "21.770000";
				sort = 690305;
				m2widx = 1;
				grade = -1;
				m0fx = "4654.529785";
				m0ttip = 690315;
				m1pat = 1;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 690302;
				gtype = 1;
				area = 690301;
				goal = 690304;
				m0fz = "-563.750000";
				m0widx = 1;
				m1ttip = 690316;
				m2fy = "89.489998";
				m2pat = 1;
				stype = 1;
				m0fy = "-58.830002";
				m0pat = 1;
				m1fz = "-658.940002";
				m2fx = "4574.120117";
				m2ttip = 690317;
			}
		}
	}
}

