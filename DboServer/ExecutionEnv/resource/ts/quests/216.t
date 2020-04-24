CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 216;
	title = 21602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 21608;
				gtype = 1;
				area = 21601;
				goal = 21604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 21605;
				stype = 1;
				taid = 1;
				title = 21602;
			}
			CDboTSActNPCConv
			{
				conv = 21609;
				ctype = 1;
				idx = 4211101;
				taid = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 21614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 21601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511203;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 21607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 8;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				not = 0;
				or = "204;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 21608;
			gtype = 1;
			oklnk = 2;
			area = 21601;
			goal = 21604;
			sort = 21605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 21602;
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
	}
}

