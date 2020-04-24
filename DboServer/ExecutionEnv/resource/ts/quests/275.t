CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 275;
	title = 27502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 27509;
				ctype = 1;
				idx = 4751201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 27508;
				gtype = 1;
				area = 27501;
				goal = 27504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 27505;
				stype = 1;
				taid = 1;
				title = 27502;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 27507;
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
				npcidx = "4751201;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				not = 0;
				or = "256;257;";
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
			cont = 27508;
			gtype = 1;
			oklnk = 2;
			area = 27501;
			goal = 27504;
			sort = 27505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 27502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 27514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 27501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511503;";
			}
		}
	}
}

