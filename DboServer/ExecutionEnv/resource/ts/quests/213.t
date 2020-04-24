CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 213;
	title = 21302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 4;
			idx = 2131;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
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
			stdiag = 21307;
			nolnk = 253;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4752202;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				not = 0;
				or = "212;";
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
				conv = 21309;
				ctype = 1;
				idx = 4752202;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 1413101;
				cnt0 = 5;
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
				cont = 21308;
				m1fx = "3460.810059";
				m1fy = "0.000000";
				sort = 21305;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2237.229980";
				grade = 132203;
				m0fx = "3233.919922";
				m0ttip = 21315;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 21302;
				gtype = 2;
				area = 21301;
				goal = 21304;
				m0fz = "-2191.129883";
				m0widx = 1;
				m1ttip = 21315;
				scitem = -1;
				stype = 2;
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
				idx0 = 1413101;
				cnt0 = 6;
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
			cont = 21308;
			gtype = 2;
			oklnk = 2;
			area = 21301;
			goal = 21304;
			sort = 21305;
			prelnk = "4;4;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 21302;
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
			desc = 21314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 21301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752202;";
			}
		}
	}
}

