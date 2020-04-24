CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 668;
	title = 66802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 101;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "4141101;";
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 5;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "4141101;";
			}
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 66808;
			gtype = 2;
			oklnk = 2;
			area = 66801;
			goal = 66804;
			sort = 66805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 66802;
		}
		CDboTSContReward
		{
			canclnk = 101;
			cid = 100;
			rwdzeny = 0;
			desc = 66814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 66801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "101;5;";
			usetbl = 1;
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
			stdiag = 66807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "1311202;";
			}
			CDboTSCheckClrQst
			{
				and = "635;";
				flink = 1;
				flinknextqid = "669;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92024;
				cnt1 = 6;
				eitype = 1;
				idx0 = 684;
				cnt0 = 8;
				ectype = 0;
				esctype = 0;
				idx2 = -1;
				cnt2 = 0;
				etype = 0;
				idx1 = 685;
				taid = 3;
			}
			CDboTSActNPCConv
			{
				conv = 66809;
				ctype = 1;
				idx = 1311202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 66808;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 66805;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "6324.410156";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 66801;
				goal = 66804;
				m0fz = "752.960022";
				m0widx = 1;
				m1ttip = 66816;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "751.859985";
				m3ttip = 66816;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "752.960022";
				m2fx = "6324.410156";
				m2ttip = 66815;
				m0fx = "5464.299805";
				m0ttip = 66815;
				m3pat = 2;
				title = 66802;
				m1fx = "5464.299805";
				m2fz = "751.859985";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 66807;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 685;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 684;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
	}
}

