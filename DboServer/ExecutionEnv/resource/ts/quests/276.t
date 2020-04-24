CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 276;
	title = 27602;

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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 27607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 12;
			}
			CDboTSClickNPC
			{
				npcidx = "4511503;";
			}
			CDboTSCheckClrQst
			{
				and = "275;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 27609;
				ctype = 1;
				idx = 4511503;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 4881103;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 4881102;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 27608;
				m3widx = 6;
				m1fy = "0.000000";
				sort = 27605;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 6;
				m3fx = "-242.789993";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				area = 27601;
				goal = 27604;
				m0fz = "1150.890015";
				m0widx = 6;
				m1ttip = 27616;
				m2fy = "0.000000";
				m2pat = 0;
				m3fz = "931.820007";
				m3ttip = 27616;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "931.489990";
				m2fx = "4361.000000";
				m2ttip = 27617;
				m0fx = "-464.089996";
				m0ttip = 27615;
				m3pat = 2;
				title = 27602;
				m1fx = "-538.729980";
				m2fz = "-3444.000000";
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
				cnt1 = 8;
				eitype = 0;
				idx0 = 4881103;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = 4881102;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 27614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 27601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511503;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 27608;
			gtype = 3;
			oklnk = 2;
			area = 27601;
			goal = 27604;
			sort = 27605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 27602;
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
	}
}

