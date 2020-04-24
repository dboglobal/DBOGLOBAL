CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2693;
	title = 269302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;4;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 269309;
				ctype = 1;
				idx = 4691101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 269308;
				gtype = 3;
				area = 269301;
				goal = 269304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 269305;
				stype = 2;
				taid = 1;
				title = 269302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 269307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "4691101;";
			}
			CDboTSCheckClrQst
			{
				and = "2692;";
				flink = 1;
				flinknextqid = "2702;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
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
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 535;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 269314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 269301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5553103;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 269308;
			gtype = 3;
			oklnk = 2;
			area = 269301;
			goal = 269304;
			sort = 269305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 269302;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 2693;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
	}
}

