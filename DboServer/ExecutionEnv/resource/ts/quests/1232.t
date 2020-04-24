CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 5;
	rq = 0;
	tid = 1232;
	title = 123202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActETimerE
			{
				taeid = 2;
				taid = 2;
				tceid = 3;
			}
			CDboTSActWorldPlayScript
			{
				sid = 1232;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActETimerS
			{
				sort = 1;
				taid = 2;
				tgid = 11;
				time = 2000;
			}
			CDboTSActWorldPlayScript
			{
				sid = 1232;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 284;
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
				conv = 123209;
				ctype = 1;
				idx = 6412101;
				taid = 2;
			}
			CDboTSActTWaitTS
			{
				taid = 3;
				time = 2000;
			}
			CDboTSActRegQInfo
			{
				cont = 123208;
				gtype = 3;
				area = 123201;
				goal = 123204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 123205;
				stype = 1;
				taid = 1;
				title = 123202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 123214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 123201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8512118;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 123207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSClickNPC
			{
				npcidx = "6412101;";
			}
			CDboTSCheckClrQst
			{
				and = "1220;";
				flink = 1;
				not = 0;
			}
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
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 123208;
			gtype = 3;
			oklnk = 2;
			area = 123201;
			goal = 123204;
			sort = 123205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 123202;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;3;2;";
		}
	}
	CNtlTSGroup
	{
		gid = 11;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
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
			stdiag = 123207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckDistWithNPC
			{
				dist = "30.000000";
				idxlist = "3362101;";
			}
		}
	}
}

