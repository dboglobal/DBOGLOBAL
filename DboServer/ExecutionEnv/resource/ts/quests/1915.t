CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 1915;
	title = 191502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSItemIdentity
			{
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 191507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "8032113;";
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
				conv = 191509;
				ctype = 1;
				idx = 8032113;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 191508;
				gtype = 2;
				area = 191501;
				goal = 191504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 191505;
				stype = 64;
				taid = 1;
				title = 191502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 191508;
			gtype = 2;
			oklnk = 2;
			area = 191501;
			goal = 191504;
			sort = 191505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 191502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 191514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 191501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8032113;";
			}
		}
	}
}

