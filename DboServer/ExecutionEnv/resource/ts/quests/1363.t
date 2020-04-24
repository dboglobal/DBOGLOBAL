CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1363;
	title = 136302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 136309;
				ctype = 1;
				idx = 4372206;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 136308;
				gtype = 1;
				area = 136301;
				goal = 136304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 136305;
				stype = 2;
				taid = 1;
				title = 136302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 136314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 136301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071109;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 136307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 44;
			}
			CDboTSClickNPC
			{
				npcidx = "4372206;";
			}
			CDboTSCheckClrQst
			{
				and = "1361;";
				flink = 1;
				flinknextqid = "1364;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 136308;
			gtype = 1;
			oklnk = 2;
			area = 136301;
			goal = 136304;
			sort = 136305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 136302;
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

