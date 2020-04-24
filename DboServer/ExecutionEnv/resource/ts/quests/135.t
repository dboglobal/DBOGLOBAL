CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 135;
	title = 13502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 13507;
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
				minlvl = 17;
			}
			CDboTSClickNPC
			{
				npcidx = "4072122;";
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
				conv = 13509;
				ctype = 1;
				idx = 4072122;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 13508;
				gtype = 1;
				area = 13501;
				goal = 13504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 13505;
				stype = 2;
				taid = 1;
				title = 13502;
			}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 13508;
			gtype = 1;
			oklnk = 2;
			area = 13501;
			goal = 13504;
			sort = 13505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 13502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 13514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 13501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142203;";
			}
		}
	}
}

