CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2618;
	title = 261802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 1257;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 1256;
				iprob1 = "1.000000";
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
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
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 261809;
				ctype = 1;
				idx = 1551101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 261808;
				gtype = 1;
				area = 261801;
				goal = 261804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 261805;
				stype = 2;
				taid = 1;
				title = 261802;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 2;
				idx0 = 1256;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1257;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 261814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 261801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1256;
			}
			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1257;
			}
			CDboTSClickNPC
			{
				npcidx = "6311102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 261808;
			gtype = 1;
			oklnk = 2;
			area = 261801;
			goal = 261804;
			sort = 261805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 261802;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 261807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				and = "2617;";
				flink = 1;
				flinknextqid = "2619;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1551101;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 1257;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 1256;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 261807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

