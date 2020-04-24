CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 145;
	title = 14502;

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
			prelnk = "100;";
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
			stdiag = 14507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "5062103;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 14514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 14501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "3;";
				widx = 1;
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
				conv = 14509;
				ctype = 1;
				idx = 5062103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 14508;
				gtype = 1;
				area = 14501;
				goal = 14504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 14505;
				stype = 2;
				taid = 1;
				title = 14502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 14508;
			gtype = 1;
			oklnk = 2;
			area = 14501;
			goal = 14504;
			sort = 14505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 14502;
		}
	}
}

